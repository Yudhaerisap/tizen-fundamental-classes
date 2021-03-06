/*
 * Tizen Fundamental Classes - TFC
 * Copyright (c) 2016-2017 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *    Components/SimpleWebView.h
 *
 * Experimental component to render text with basic HTML tag without needing
 * of using EWK widget
 *
 * Created on:  Mar 17, 2016
 * Author:      Gilang Mentari Hamidy (g.hamidy@samsung.com)
 * Contributor: Kevin Winata (k.winata@samsung.com)
 */

#ifndef TFC_COMP_SIMPLEWEBVIEW_H_
#define TFC_COMP_SIMPLEWEBVIEW_H_

#include "TFC/Components/ComponentBase.h"
#include "TFC/Async.h"

/**
 * Default font size.
 */
#define SIMPLE_WEB_VIEW_FONT_SIZE 32

namespace TFC {
namespace Components {

/**
 * Component that acts as a simple HTML parser and renderer.
 * It is an alternative to EWebKit that's provided, but with more control over its parsing and rendering method.
 * Supported tags include : br, img, strong, span, em, and p.
 * You can also add custom handling of unsupported HTML tags by attaching a callback.
 */
class LIBAPI SimpleWebView:
		public ComponentBase,
		EventEmitterClass<SimpleWebView>,
		PropertyClass<SimpleWebView>
{
	using EventEmitterClass<SimpleWebView>::Event;
	using PropertyClass<SimpleWebView>::Property;
public:
	/**
	 * Constructor of SimpleWebView component.
	 */
	SimpleWebView();

	/**
	 * Destructor of SimpleWebView component.
	 */
	~SimpleWebView();

	/**
	 * Method to set HTML content that will be parsed. It will also start the rendering process.
	 *
	 * @param data HTML content.
	 */
	void SetHTMLData(const std::string& data);

	/**
	 * Method to append any widget to the main box.
	 * The widget will be placed immediately after the last rendered content,
	 * so if you want to place the widget in the middle, you have to call this method
	 * from eventProcessingCustomTag's callback function.
	 *
	 * @param component Elementary widget that will be appended.
	 */
	void AddCustomComponent(Evas_Object* component);

	/**
	 * Event that will be triggered on every discovery of unsupported HTML tags.
	 * It will contains a string pointer to the content between the unknown tag opening and closing.
	 * You can attach a callback to this event to process the content yourself,
	 * and call AddCustomComponent() to represent the content as a custom widget.
	 */
	Event<std::string*> eventProcessingCustomTag;
protected:
	/**
	 * Method overriden from ComponentBase, creates the UI elements of the component.
	 *
	 * @param root The root/parent given for this component.
	 *
	 * @return A box (Elm_Box) that contains the rendered result.
	 */
	virtual Evas_Object* CreateComponent(Evas_Object* root) override;
private:
	std::string data;
	std::string fontFormat;
	std::string font;

	struct ImageAsyncPackage {
		Evas_Object* placeholder;
		std::string filePath;
	};

	Async<ImageAsyncPackage>::Event eventImageDownloadCompleted;

	int fontSize;

	Evas_Object* box;
	Evas_Object* boxPage;

	bool isRendering;

	void SetFont(std::string const& font);
	std::string const& GetFont() const;

	void SetFontSize(int const& fontSize);
	int GetFontSize() const;

	void AddParagraph(Evas_Object* boxPage, std::string& paragraph);
	void AddImage(std::string& url);
	void Render();

	void OnImageDownloadCompleted(Async<ImageAsyncPackage>::Task* asyncTask, ImageAsyncPackage result);
public:
	/**
	 * Property that can be used to getting & setting the font family of the rendered text.
	 */
	Property<std::string const&>::Get<&SimpleWebView::GetFont>::Set<&SimpleWebView::SetFont> Font;

	/**
	 * Property that can be used to getting & setting the font size of the rendered text.
	 */
	Property<int>::Get<&SimpleWebView::GetFontSize>::Set<&SimpleWebView::SetFontSize> FontSize;
};


}
}



#endif /* SIMPLEWEBVIEW_H_ */
